import styles from "@site/src/pages/download/index.module.css";
import Link from "@docusaurus/Link";
import {JSX} from "react";

type VersionInfoProps = {
    version: string,
    date: string,
    changes: JSX.Element,
    supportOld: boolean,
    buttons: JSX.Element[],
    blogLink: string,
    updatedDate?: string
}

function convertToDateTime(dateString: string) {
    const parts = dateString.split('.');

    const day = Number(parts[0]);
    const month = Number(parts[1]) - 1;
    const year = Number('20' + parts[2]);

    const date = new Date(year, month, day);

    return date.toISOString().split('T')[0];
}

const VersionInfo = ({version, date, changes, supportOld, buttons, blogLink, updatedDate}: VersionInfoProps) => {
    return (
        <div className={styles.version}>
            <div id="#version">
                <Link to={"blog/" + blogLink}><h2>LDYOM | {version}</h2></Link>
                <time dateTime={convertToDateTime(date)}>{date}</time>
                {
                    updatedDate ? <span><br/>Updated on <time dateTime={updatedDate}>{updatedDate}</time></span> : null
                }
            </div>
            <hr/>
            <div id="changelog">
                <p>Changes:</p>
                {changes}
                Support for missions from the previous version: <span
                className="positive_text">{supportOld ? "Yes" : "No"}</span>
            </div>
            <div className={styles.download_buttons}>
                {buttons}
            </div>
        </div>
    )
}

export default VersionInfo;