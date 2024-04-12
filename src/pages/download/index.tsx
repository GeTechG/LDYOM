import Layout from "@theme/Layout";
import {JSX} from "react";
import Beta0_8_0_Preview_9 from "./versions/beta0_8_0_Preview_9";
import Beta_0_7_2 from "./versions/beta_0_7_2";
import Beta_0_7_1 from "./versions/beta_0_7_1";
import Beta_0_7_0 from "./versions/beta_0_7_0";
    changes: JSX.Element,


export default function Downloads(): JSX.Element {
    return (
        <Layout
            description="Create your world">
            <div>
                <VersionInfo version="Beta 0.8.0 Preview 9" date="05.03.24"
                <Beta0_8_0_Preview_9/>
                <Beta_0_7_2/>
                <Beta_0_7_1/>
                <Beta_0_7_0/>
            </div>
        </Layout>
    );
}