import VersionInfo from "@site/src/pages/download/versionInfo";
import Link from "@docusaurus/Link";
import useBaseUrl from "@docusaurus/useBaseUrl";
import styles from "@site/src/pages/download/index.module.css";

function Beta_0_7_1() {
    return <VersionInfo version="Beta 0.7.1" date="01.05.21"
                        changes={
                            <ul>
                                <li>Ported to C++</li>
                                <li>New rendering for the node system</li>
                                <li>Added a large number of new nodes</li>
                                <li>Added trains</li>
                                <li>New parameters for cars</li>
                                <li>New parameters for actors</li>
                                <li>New Cyber theme</li>
                                <li>Backward compatibility for future versions</li>
                                <li>Fixed and added new bugs</li>
                                <li>Many minor changes</li>
                            </ul>
                        }
                        supportOld={false}
                        buttons={[
                            <Link target="_blank" to={useBaseUrl("/downloads/Beta 0.7.1.7z")}
                                  className={styles.button}>Download</Link>
                        ]}
                        blogLink="2021/05/01/beta-0-7-1"/>;
}

export default Beta_0_7_1;